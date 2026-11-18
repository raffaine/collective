import Foundation
import AVFoundation
import CoreGraphics
import CoreText
import AppKit

struct SceneSpec {
    let path: String
    let actLabel: String
    let placeLabel: String
}

let scenes: [SceneSpec] = [
    // Parte I: Cenário Delta — Santuário Perinatal, Casulo Pós-Parto de 90 Dias, Ciranda Sem Câmeras e Banco de Leite Fiocruz
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_01.png", actLabel: "PARTE I // CENÁRIO DELTA: CASULO PÓS-PARTO DE 90 DIAS", placeLabel: "Varanda de Cedro pela Manhã (Marmitas Gamma e Fraldas a 71°C)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_02.png", actLabel: "PARTE I // CENÁRIO DELTA: BERÇÁRIO SEM CÂMERAS E 2 ADULTOS", placeLabel: "Ciranda Infantil Sem Câmera (Válvula Anti-Escaldadura até 38°C)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_03.png", actLabel: "PARTE I // CENÁRIO DELTA: DESCOBERTA E BANCO DE LEITE", placeLabel: "Ponte do Riacho e Trilha (Banco de Leite Fiocruz a 62,5°C)"),

    // Parte II: Cenário Omicron — Pactos de Parentesco Escolhido, Multiparentalidade STF/CNJ, Anti-Moralismo e Saída em menos de 500 ms
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_04.png", actLabel: "PARTE II // CENÁRIO OMICRON: PARENTESCO ESCOLHIDO", placeLabel: "Pérgola Florida e Lousa de Pacto Familiar (Multiparentalidade STF/CNJ)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_05.png", actLabel: "PARTE II // CENÁRIO OMICRON: CHAVE DE SANTUÁRIO E ANTI-MORALISMO", placeLabel: "Vestíbulo Familiar e Chave-Faca R1 (Firewall Anti-Moralismo)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_06.png", actLabel: "PARTE II // CENÁRIO OMICRON: SAÍDA DE PORTO SEGURO", placeLabel: "Suíte de Refúgio Sem Vigilância (Saída Unilateral em menos de 500 ms)"),

    // Parte III: Cenário Kappa — Mestria Polímata Sem Dívida, BKT Privado em R0, Coleira BLE de 2m e Banca 3-de-3
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_07.png", actLabel: "PARTE III // CENÁRIO KAPPA: MESTRIA BKT PRIVADA EM R0", placeLabel: "Bancada de Aprendiz e Coleira BLE de 2 Metros do Mentor (BKT a 95%)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_08.png", actLabel: "PARTE III // CENÁRIO KAPPA: BANCA 3-DE-3 ANTI-NEPOTISMO", placeLabel: "Galeria da Guilda e Pedestal de Co-Assinatura 3-de-3 (LDB Art. 23/24)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_09.png", actLabel: "PARTE III // CENÁRIO KAPPA: MOBILIDADE POLÍMATA", placeLabel: "Claustro da Botica Botânica e Sopro de Vidro (Graus T1 a T4)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_10.png", actLabel: "PARTE III // CENÁRIO KAPPA: RECONHECIMENTO DO MENTOR", placeLabel: "Biblioteca da Guilda à Beira-Lago e Estaleiro (Zero Dívida Estudantil)"),

    // Parte IV: Cenário Zeta — Moradia Descomodificada TTC/CDRU, Casas R0 100% Sem Telas e Casa de Banhos Regenerativa
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_11.png", actLabel: "PARTE IV // CENÁRIO ZETA: MORADIA TTC E CDRU SEM ALUGUEL", placeLabel: "Casas de Pátio ao Crepúsculo (Aluguel Zero, TTC e Bem de Família)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_12.png", actLabel: "PARTE IV // CENÁRIO ZETA: FRONTEIRA R0_INTERIOR_DARK", placeLabel: "Sala 100% Sem Telas nem Microfones e Interruptores Táteis de Latão"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_13.png", actLabel: "PARTE IV // CENÁRIO ZETA: CASA DE BANHOS REGENERATIVA", placeLabel: "Sauna de Cedro e Piscina Térmica Mineral (Hidroterapia Comunitária)"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_14.png", actLabel: "PARTE IV // CENÁRIO ZETA: RECUPERAÇÃO DWHR E CALOR DIGITAL", placeLabel: "Serpentinas de Cobre DWHR (ao menos 55%) e Calor dos Servidores"),
    SceneSpec(path: "doc/podcast/renders/ep06/scenes/scene_15.png", actLabel: "PARTE IV // CENÁRIO ZETA: JARDINS FILTRANTES DE 72 HORAS", placeLabel: "Passarela do Jardim Filtrante de Lírios e Câmaras a -25 Pa Sem Odor")
]

let width = 1920
let height = 1080
let fps: Int32 = 8
let crossfadeSec: Double = 4.0
let colorSpace = CGColorSpaceCreateDeviceRGB()
let bitmapInfo = CGImageAlphaInfo.premultipliedFirst.rawValue | CGBitmapInfo.byteOrder32Little.rawValue

func loadNormalizedCGImage(_ path: String) -> CGImage {
    guard let nsImg = NSImage(contentsOfFile: path),
          let srcCG = nsImg.cgImage(forProposedRect: nil, context: nil, hints: nil) else {
        fatalError("Failed to load image at \(path)")
    }
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create normalize CGContext")
    }
    ctx.interpolationQuality = .high
    let scaleX = CGFloat(width) / CGFloat(srcCG.width)
    let scaleY = CGFloat(height) / CGFloat(srcCG.height)
    let scale = max(scaleX, scaleY)
    let drawW = CGFloat(srcCG.width) * scale
    let drawH = CGFloat(srcCG.height) * scale
    let dx = (CGFloat(width) - drawW) * 0.5
    let dy = (CGFloat(height) - drawH) * 0.5
    ctx.draw(srcCG, in: CGRect(x: dx, y: dy, width: drawW, height: drawH))
    return ctx.makeImage()!
}

func makeHudCGImage(actLabel: String, placeLabel: String) -> CGImage {
    guard let ctx = CGContext(
        data: nil,
        width: width,
        height: height,
        bitsPerComponent: 8,
        bytesPerRow: width * 4,
        space: colorSpace,
        bitmapInfo: bitmapInfo
    ) else {
        fatalError("Cannot create HUD CGContext")
    }

    let boxRect = CGRect(x: 48, y: 42, width: 920, height: 86)
    let boxPath = CGPath(roundedRect: boxRect, cornerWidth: 8, cornerHeight: 8, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.64))
    ctx.addPath(boxPath)
    ctx.fillPath()

    ctx.setStrokeColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.45))
    ctx.setLineWidth(1.5)
    ctx.addPath(boxPath)
    ctx.strokePath()

    let pillRect = CGRect(x: 48, y: height - 66, width: 780, height: 34)
    let pillPath = CGPath(roundedRect: pillRect, cornerWidth: 6, cornerHeight: 6, transform: nil)
    ctx.setFillColor(CGColor(red: 0.06, green: 0.08, blue: 0.10, alpha: 0.58))
    ctx.addPath(pillPath)
    ctx.fillPath()

    func drawText(_ text: String, font: CTFont, color: CGColor, x: CGFloat, y: CGFloat) {
        let attrs: [NSAttributedString.Key: Any] = [
            NSAttributedString.Key(kCTFontAttributeName as String): font,
            NSAttributedString.Key(kCTForegroundColorAttributeName as String): color
        ]
        let attrStr = NSAttributedString(string: text, attributes: attrs)
        let line = CTLineCreateWithAttributedString(attrStr)
        ctx.textPosition = CGPoint(x: x, y: y)
        CTLineDraw(line, ctx)
    }

    let fontTop = CTFontCreateWithName("Courier-Bold" as CFString, 15, nil)
    let fontAct = CTFontCreateWithName("Courier-Bold" as CFString, 15, nil)
    let fontPlace = CTFontCreateWithName("Helvetica-Bold" as CFString, 20, nil)

    let gold = CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.95)
    let green = CGColor(red: 0.48, green: 0.82, blue: 0.42, alpha: 0.95)
    let white = CGColor(red: 0.95, green: 0.96, blue: 0.98, alpha: 0.96)

    drawText("RÁDIO LAMBDA BRASIL // EP. 06: ONDA C (DELTA, OMICRON, KAPPA E ZETA)", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/pt-br/episodes/06.episode.m4a"
        let finalOutPath = "doc/podcast/pt-br/episodes/06.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ptbr_ep06_silent_video.mp4"

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        try? FileManager.default.removeItem(atPath: finalOutPath)

        let audioURL = URL(fileURLWithPath: audioPath)
        let audioAsset = AVURLAsset(url: audioURL)
        let durationCM = try await audioAsset.load(.duration)
        let durationSec = CMTimeGetSeconds(durationCM)
        print(String(format: "Audio duration: %.2f seconds (%.2f minutes)", durationSec, durationSec / 60.0))

        let cgScenes = scenes.map { loadNormalizedCGImage($0.path) }
        let hudScenes = scenes.map { makeHudCGImage(actLabel: $0.actLabel, placeLabel: $0.placeLabel) }
        let sceneDuration = durationSec / Double(scenes.count)

        for (i, s) in scenes.enumerated() {
            let startSec = Int(round(Double(i) * sceneDuration))
            let mm = startSec / 60
            let ss = startSec % 60
            print(String(format: "CHAPTER %02d:%02d — %@ (%@)", mm, ss, s.placeLabel, s.actLabel))
        }

        let writer = try AVAssetWriter(outputURL: URL(fileURLWithPath: tempVideoPath), fileType: .mp4)
        let videoSettings: [String: Any] = [
            AVVideoCodecKey: AVVideoCodecType.h264,
            AVVideoWidthKey: width,
            AVVideoHeightKey: height,
            AVVideoCompressionPropertiesKey: [
                AVVideoAverageBitRateKey: 1_800_000,
                AVVideoMaxKeyFrameIntervalKey: 16,
                AVVideoProfileLevelKey: AVVideoProfileLevelH264HighAutoLevel
            ]
        ]
        let writerInput = AVAssetWriterInput(mediaType: .video, outputSettings: videoSettings)
        writerInput.expectsMediaDataInRealTime = false

        let pixelBufferAttrs: [String: Any] = [
            kCVPixelBufferPixelFormatTypeKey as String: Int(kCVPixelFormatType_32BGRA),
            kCVPixelBufferWidthKey as String: width,
            kCVPixelBufferHeightKey as String: height,
            kCVPixelBufferCGImageCompatibilityKey as String: true,
            kCVPixelBufferCGBitmapContextCompatibilityKey as String: true
        ]
        let adaptor = AVAssetWriterInputPixelBufferAdaptor(
            assetWriterInput: writerInput,
            sourcePixelBufferAttributes: pixelBufferAttrs
        )

        writer.add(writerInput)
        writer.startWriting()
        writer.startSession(atSourceTime: .zero)

        let totalFrames = Int(ceil(durationSec * Double(fps)))
        print("Rendering \(totalFrames) frames at \(fps) fps across \(scenes.count) scenes...")

        func drawSceneKenBurns(_ ctx: CGContext, img: CGImage, index: Int, progress: Double, alpha: CGFloat) {
            let p = CGFloat(max(0.0, min(1.0, progress)))
            let zoomIn = (index % 2 == 0)
            let scale: CGFloat = zoomIn ? (1.02 + 0.08 * p) : (1.10 - 0.08 * p)
            let panDir: CGFloat = (index % 3 == 0) ? 1.0 : ((index % 3 == 1) ? -1.0 : 0.4)
            let maxPanX = CGFloat(width) * 0.025
            let maxPanY = CGFloat(height) * 0.015
            let offsetX = panDir * maxPanX * (p - 0.5)
            let offsetY = -panDir * maxPanY * (p - 0.5)

            let drawW = CGFloat(width) * scale
            let drawH = CGFloat(height) * scale
            let x = (CGFloat(width) - drawW) * 0.5 + offsetX
            let y = (CGFloat(height) - drawH) * 0.5 + offsetY

            ctx.saveGState()
            if alpha < 0.999 {
                ctx.setAlpha(alpha)
            }
            ctx.interpolationQuality = .low
            ctx.draw(img, in: CGRect(x: x, y: y, width: drawW, height: drawH))
            ctx.restoreGState()
        }

        for frameIdx in 0..<totalFrames {
            while !writerInput.isReadyForMoreMediaData {
                usleep(2000)
            }

            autoreleasepool {
                let t = Double(frameIdx) / Double(fps)
                let rawIdx = Int(t / sceneDuration)
                let curIdx = min(max(0, rawIdx), scenes.count - 1)
                let sceneLocalT = t - Double(curIdx) * sceneDuration
                let prog = sceneLocalT / sceneDuration

                var pixelBufferOut: CVPixelBuffer?
                CVPixelBufferPoolCreatePixelBuffer(nil, adaptor.pixelBufferPool!, &pixelBufferOut)
                guard let pixelBuffer = pixelBufferOut else { return }

                CVPixelBufferLockBaseAddress(pixelBuffer, [])
                let baseAddr = CVPixelBufferGetBaseAddress(pixelBuffer)!
                let bytesPerRow = CVPixelBufferGetBytesPerRow(pixelBuffer)

                let ctx = CGContext(
                    data: baseAddr,
                    width: width,
                    height: height,
                    bitsPerComponent: 8,
                    bytesPerRow: bytesPerRow,
                    space: colorSpace,
                    bitmapInfo: bitmapInfo
                )!

                drawSceneKenBurns(ctx, img: cgScenes[curIdx], index: curIdx, progress: prog, alpha: 1.0)

                if curIdx + 1 < scenes.count && (sceneDuration - sceneLocalT) < crossfadeSec {
                    let fadeAlpha = CGFloat((crossfadeSec - (sceneDuration - sceneLocalT)) / crossfadeSec)
                    drawSceneKenBurns(ctx, img: cgScenes[curIdx + 1], index: curIdx + 1, progress: 0.0, alpha: fadeAlpha)
                }

                ctx.interpolationQuality = .none
                ctx.draw(hudScenes[curIdx], in: CGRect(x: 0, y: 0, width: width, height: height))

                let totalProg = CGFloat(t / durationSec)
                ctx.setFillColor(CGColor(red: 1.0, green: 1.0, blue: 1.0, alpha: 0.18))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width), height: 6))
                ctx.setFillColor(CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.92))
                ctx.fill(CGRect(x: 0, y: 0, width: CGFloat(width) * totalProg, height: 6))

                CVPixelBufferUnlockBaseAddress(pixelBuffer, [])

                let pts = CMTime(value: CMTimeValue(frameIdx), timescale: fps)
                adaptor.append(pixelBuffer, withPresentationTime: pts)
            }

            if frameIdx % 1200 == 0 && frameIdx > 0 {
                let pct = Int((Double(frameIdx) / Double(totalFrames)) * 100.0)
                print("Progress: \(pct)% (\(frameIdx)/\(totalFrames) frames)")
            }
        }

        writerInput.markAsFinished()
        await writer.finishWriting()
        print("Silent video track rendered. Now muxing audio losslessly...")

        let composition = AVMutableComposition()
        let videoAsset = AVURLAsset(url: URL(fileURLWithPath: tempVideoPath))

        let compVideoTrack = composition.addMutableTrack(withMediaType: .video, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcVideoTrack = try await videoAsset.loadTracks(withMediaType: .video).first!
        try compVideoTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcVideoTrack, at: .zero)

        let compAudioTrack = composition.addMutableTrack(withMediaType: .audio, preferredTrackID: kCMPersistentTrackID_Invalid)!
        let srcAudioTrack = try await audioAsset.loadTracks(withMediaType: .audio).first!
        try compAudioTrack.insertTimeRange(CMTimeRange(start: .zero, duration: durationCM), of: srcAudioTrack, at: .zero)

        let exporter = AVAssetExportSession(asset: composition, presetName: AVAssetExportPresetPassthrough)!
        try await exporter.export(to: URL(fileURLWithPath: finalOutPath), as: .mp4)

        try? FileManager.default.removeItem(atPath: tempVideoPath)
        print("SUCCESS: Created PT-BR Episode 06 YouTube video at \(finalOutPath)")
        semaphore.signal()
    } catch {
        print("ERROR: \(error)")
        exit(1)
    }
}

semaphore.wait()
