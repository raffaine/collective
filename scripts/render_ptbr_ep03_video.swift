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
    // Parte I: Camada 5 — Ostrom (8 Princípios, Quilombos e Faxinais), VSM Sistema 5 e Parentesco Sem Tokens
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_01.png", actLabel: "PARTE I // CAMADA 5: COMUNS DE OSTROM E PARENTESCO", placeLabel: "Oficina ao Anoitecer (Escala N=1 Sem Reuniões Inúteis)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_02.png", actLabel: "PARTE I // CAMADA 5: COMUNS DE OSTROM E PARENTESCO", placeLabel: "Varanda de Ferramentas e Marmitas (8 Princípios e Quilombos)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_03.png", actLabel: "PARTE I // CAMADA 5: COMUNS DE OSTROM E PARENTESCO", placeLabel: "Pavilhão Circular de Madeira (VSM Sistema 5 e Anéis R0 a R6)"),

    // Parte II: Camada 5 — Defesa Anti-Sybil 25x25 (EigenTrust Q16.16) e Disjuntor Anti-Martírio de 40%
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_04.png", actLabel: "PARTE II // DEFESA ANTI-SYBIL 25X25 E DISJUNTOR 40%", placeLabel: "Passarela da Árvore-Mãe (Veto Ecológico e Corte Máximo Sybil)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_05.png", actLabel: "PARTE II // DEFESA ANTI-SYBIL 25X25 E DISJUNTOR 40%", placeLabel: "Pátio do Mutirão de Carpintaria (Disjuntor Anti-Martírio de 40%)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_06.png", actLabel: "PARTE II // DEFESA ANTI-SYBIL 25X25 E DISJUNTOR 40%", placeLabel: "Cozinha Comunitária e Lousa E-Ink Lavável (Rotação de Cuidado)"),

    // Parte III: Camada 6 — Ontologias Plurais (Grafos RDF com Bem Viver Guarani Teko Porã e Yanomami Urihi a)
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_07.png", actLabel: "PARTE III // CAMADA 6: ONTOLOGIAS PLURAIS E TEKO PORÃ", placeLabel: "Trilha Verde do Vale (Grafos RDF Locais Sem Colonialismo)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_08.png", actLabel: "PARTE III // CAMADA 6: ONTOLOGIAS PLURAIS E TEKO PORÃ", placeLabel: "Vestíbulo da Clínica e Cordão Vermelho (Bem Viver e Urihi a)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_09.png", actLabel: "PARTE III // CAMADA 6: ONTOLOGIAS PLURAIS E TEKO PORÃ", placeLabel: "Sala de Madeira Sem Telas e Lareira (Silêncio Digital em R0)"),

    // Parte IV: Camada 6 — Correção do Bug do Pipe, DAGs Estequiométricos Sem Lixo e Voz do Dissidente
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_10.png", actLabel: "PARTE IV // DAGS ESTEQUIOMÉTRICOS E VOZ DO DISSIDENTE", placeLabel: "Salão Acústico e Chave-Faca de Latão (Zero Colisão de Hash)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_11.png", actLabel: "PARTE IV // DAGS ESTEQUIOMÉTRICOS E VOZ DO DISSIDENTE", placeLabel: "Café da Esquina e Portal Vizinho Zero-ID (DAGs Sem Resíduo Órfão)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_12.png", actLabel: "PARTE IV // DAGS ESTEQUIOMÉTRICOS E VOZ DO DISSIDENTE", placeLabel: "Parque de Desconstrução Urbana (Co-Assinatura do Dissidente)"),

    // Parte V: Camada 7 Brasileira — Extração Circular Saudável, Art. 5º XVIII CF/88, Ato Cooperativo, TTC e RPPN
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_13.png", actLabel: "PARTE V // CAMADA 7 BRASILEIRA E EXTRAÇÃO CIRCULAR", placeLabel: "Bosque do Termo Territorial Coletivo (CF/88, Ato Cooperativo e RPPN)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_14.png", actLabel: "PARTE V // CAMADA 7 BRASILEIRA E EXTRAÇÃO CIRCULAR", placeLabel: "Pavilhão de Chaves Seletoras (Compras Coletivas e Sifão de 10% a 95%)"),
    SceneSpec(path: "doc/podcast/renders/ep03/scenes/scene_15.png", actLabel: "PARTE V // CAMADA 7 BRASILEIRA E EXTRAÇÃO CIRCULAR", placeLabel: "Mirante do Farol Planetário (Quando a Casca Velha Se Dissolve)")
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

    let boxRect = CGRect(x: 48, y: 42, width: 900, height: 86)
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
    let fontPlace = CTFontCreateWithName("Helvetica-Bold" as CFString, 21, nil)

    let gold = CGColor(red: 0.96, green: 0.77, blue: 0.19, alpha: 0.95)
    let green = CGColor(red: 0.48, green: 0.82, blue: 0.42, alpha: 0.95)
    let white = CGColor(red: 0.95, green: 0.96, blue: 0.98, alpha: 0.96)

    drawText("RÁDIO LAMBDA BRASIL // EP. 03: A ALMA DA MÁQUINA (OSTROM, TEKO PORÃ E CAMADA 7)", font: fontTop, color: gold, x: 64, y: CGFloat(height - 54))
    drawText(actLabel, font: fontAct, color: green, x: 68, y: 96)
    drawText(placeLabel, font: fontPlace, color: white, x: 68, y: 62)

    return ctx.makeImage()!
}

let semaphore = DispatchSemaphore(value: 0)

Task {
    do {
        let audioPath = "doc/podcast/pt-br/episodes/03.episode.m4a"
        let finalOutPath = "doc/podcast/pt-br/episodes/03.episode_youtube.mp4"
        let tempVideoPath = "/tmp/ptbr_ep03_silent_video.mp4"

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
        print("SUCCESS: Created PT-BR Episode 03 YouTube video at \(finalOutPath)")
        semaphore.signal()
    } catch {
        print("ERROR: \(error)")
        exit(1)
    }
}

semaphore.wait()
